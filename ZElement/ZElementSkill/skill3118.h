#ifndef __CPPGEN_GNET_SKILL3118
#define __CPPGEN_GNET_SKILL3118
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill3118:public Skill
    {
      public:
        enum
        { SKILL_ID = 3118 };
          Skill3118 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill3118Stub:public SkillStub
    {
      public:
#ifdef _SKILL_SERVER
        class State1:public SkillStub::State
        {
          public:
            int GetTime (Skill * skill) const
            {
                return 800;
            }
            void Calculate (Skill * skill) const
            {
                skill->GetPlayer ()->SetPray (1);
            }
        };
#endif
#ifdef _SKILL_SERVER
        class State2:public SkillStub::State
        {
          public:
            int GetTime (Skill * skill) const
            {
                return 200;
            }
            void Calculate (Skill * skill) const
            {
                skill->GetPlayer ()->SetVar2 (skill->GetPlayer ()->GetHp () * 1000 / skill->GetPlayer ()->GetMaxhp ());
                skill->GetPlayer ()->SetVar1 (skill->GetPlayer ()->GetRes1 ());
                skill->GetPlayer ()->SetPerform (1);
            }
        };
#endif
      Skill3118Stub ():SkillStub (3118)
        {
            occupation = 169;
            name = L"øÒ¡È∑¥ …<Ï¯>";
            nativename = "øÒ¡È∑¥ …<Ï¯>";
            icon = "∑°§øÒ¡È∑¥ ….dds";
            maxlevel = 20;
            maxlearn = 20;
            type = 2;
            eventflag = 0;
            spcost = 0;
            timetype = 0;
            targettype = 0;
            autoattack = 0;
            allowform = 0;
            credittype = 0;
            clearmask = 4;
            serialskill = 0;
            charge = 0;
            chargedist = 0;
            succeedskillcolor = 1;
            succeedskillcolornum = 1;
            comboskill = 0;
            cycle = 0;
            cyclegfx = "";
            cyclemode = 0;
            skill_class = 0;
            guide = false;
            summon_id = 0;
            trigger = false;
            castInPreSkill = false;
            preCastSkillId = 0;
            darkLightType = 0;
            followskill = 0;
            targetcnt = 0;
            excludemonstercnt = 0;
            skillelemsnum = 0;
            skillelemsfactor = 0;
            preskillnum = 1;
            talent[0] = 647;
            talent[1] = 662;
            talent[2] = 666;
            talent_size = 3;
            action[0] = "πÌÕı◊⁄_øÒ¡È∑¥ …";
            action[1] = "πÌÕı◊⁄_øÒ¡È∑¥ …";
            action[2] = "πÌÕı◊⁄_øÒ¡È∑¥ …";
            action[3] = "πÌÕı◊⁄_øÒ¡È∑¥ …";
            action[4] = "πÌÕı◊⁄_øÒ¡È∑¥ …";
            action[5] = "πÌÕı◊⁄_øÒ¡È∑¥ …";
            action[6] = "0";
            action[7] = "0";
            action[8] = "0";
            action[9] = "0";
            action[10] = "0";
            action[11] = "0";
            action[12] = "0";
            action[13] = "";
            action[14] = "";
            action[15] = "πÌÕı◊⁄_∆Ô≥À_øÒ¡È∑¥ …";
            action[16] = "πÌÕı◊⁄_∆Ô≥À_øÒ¡È∑¥ …";
            action[17] = "πÌÕı◊⁄_∆Ô≥À_øÒ¡È∑¥ …";
            action[18] = "πÌÕı◊⁄_∆Ô≥À_øÒ¡È∑¥ …";
            action[19] = "πÌÕı◊⁄_∆Ô≥À_øÒ¡È∑¥ …";
            action[20] = "πÌÕı◊⁄_∆Ô≥À_øÒ¡È∑¥ …";
            action[21] = "0";
            action[22] = "0";
            action[23] = "0";
            action[24] = "0";
            action[25] = "0";
            action[26] = "0";
            action[27] = "0";
            action[28] = "";
            action[29] = "";
            action[30] = "0";
            rangetype = 5;
            doenchant = true;
            dobless = true;
            need_item = false;
            item_relation = 0;
            item1_id = -1;
            item1_num = 0;
            item2_id = -1;
            item2_num = 0;
            weapon_limit = -1;
            skill_limit = 0;
#ifdef _SKILL_SERVER
            statestub.push_back (new State1 ());
            statestub.push_back (new State2 ());
#endif
        }
        virtual ~ Skill3118Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (180 + 26 * skill->GetLevel () + 3 * skill->GetLevel () * skill->GetLevel ());
        }
        int GetPreSkillID (int index) const
        {
            static int array[1] = { 776 };
            return array[index];
        }
        int GetPreSkillSP (int index) const
        {
            static int array[1] = { 5 };
            return array[index];
        }
        int GetDivinityExp (int level) const
        {
            static int array[20] =
                { 400, 500, 500, 700, 1500, 2400, 3500, 5600, 8500, 8600, 19000, 26500, 49100, 57600, 85800, 140300, 241000, 410600, 614500,
 1344300 };
            return array[level - 1];
        }
        int GetDivinityLevel (int level) const
        {
            static int array[20] = { 2, 4, 6, 8, 10, 12, 15, 18, 21, 24, 28, 33, 38, 43, 48, 54, 60, 66, 72, 78 };
            return array[level - 1];
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (660 + 20 * skill->GetLevel ());
        }
        int GetExecutetime (Skill * skill) const
        {
            return 1000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 180000 - 18000 * skill->GetT1 ();
        }
        int GetRequiredLevel (int level) const
        {
            static int array[20] = { 120, 122, 124, 126, 128, 130, 131, 132, 133, 134, 135, 136, 137, 138, 139, 141, 143, 145, 147, 149 };
            return array[level - 1];
        }
        float GetPraydistance (Skill * skill) const
        {
            return (float) (skill->GetPlayer ()->GetRange ());
        }
        float GetCastdistance (Skill * skill) const
        {
            return (float) (skill->GetPlayer ()->GetRange ());
        }
#ifdef _SKILL_CLIENT
        int GetDescription (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format,
                               660 + 20 * skill->GetLevel (),
                               180 + 26 * skill->GetLevel () + 3 * skill->GetLevel () * skill->GetLevel (),
                               180, 20, 5 * skill->GetLevel (), 10, 2 + 0.5 * skill->GetLevel (), skill->GetLevel ());

        }
#endif
#ifdef _SKILL_CLIENT
        int GetIntroduction (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format, skill->GetShowlevel ());

        }
#endif
#ifdef _SKILL_SERVER
        bool StateAttack (Skill * skill) const
        {
            skill->GetVictim ()->SetProbability (1.0 * (100));
            skill->GetVictim ()->SetTime (20000 + 100 + 2000 * skill->GetT0 ());
            skill->GetVictim ()->SetRatio (0.05 * skill->GetLevel () + norm (skill->GetLevel () / 19.9) * 0.3);
            skill->GetVictim ()->SetAmount (0);
            skill->GetVictim ()->SetValue (5 * skill->GetLevel ());
            skill->GetVictim ()->SetRetort (1);
            skill->GetVictim ()->SetTime (10000 + 100 + 3000 * skill->GetT2 ());
            skill->GetVictim ()->SetImmunesleep (1);
            skill->GetVictim ()->SetProbability (1.0 * (skill->GetT0 () > 0 ? 120 : 0));
            skill->GetVictim ()->SetTime (60000 * skill->GetT0 ());
            skill->GetVictim ()->SetBuffid (1);
            skill->GetVictim ()->SetAmount (0.00001 + 0.1 * skill->GetT0 ());
            skill->GetVictim ()->SetValue (0.06 * skill->GetT0 () * skill->GetPlayer ()->GetMaxmp ());
            skill->GetVictim ()->SetAddhp2 (1);
            skill->GetVictim ()->SetTime ((10000 + 100 + 3000 * skill->GetT2 ()) * norm (skill->GetT2 ()));
            skill->GetVictim ()->SetImmunedizzy (1);
            return true;
        }
        bool BlessMe (Skill * skill) const
        {
            skill->GetVictim ()->SetProbability (1.0 * (100));
            skill->GetVictim ()->SetTime (2000 + 500 * skill->GetLevel ());
            skill->GetVictim ()->SetRatio (1);
            skill->GetVictim ()->SetAmount (skill->GetLevel ());
            skill->GetVictim ()->SetValue (0.5 * skill->GetPlayer ()->GetVar1 ());
            skill->GetVictim ()->SetBloodshield (1);
            skill->GetVictim ()->SetProbability (1.0 * (skill->GetPlayer ()->GetVar2 () > 950 ? 120 * norm (skill->GetT0 () / 3.9) : 0));
            skill->GetVictim ()->SetRatio (0);
            skill->GetVictim ()->SetValue (skill->GetPlayer ()->GetMaxhp ());
            skill->GetVictim ()->SetHeal (1);
            return true;
        }
#endif
    };
}
#endif
