#ifndef __CPPGEN_GNET_SKILL1124
#define __CPPGEN_GNET_SKILL1124
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill1124:public Skill
    {
      public:
        enum
        { SKILL_ID = 1124 };
          Skill1124 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill1124Stub:public SkillStub
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
                skill->GetPlayer ()->SetPerform (1);
            }
        };
#endif
      Skill1124Stub ():SkillStub (1124)
        {
            occupation = 28;
            name = L"ìå»êÑªÓ¡";
            nativename = "ìå»êÑªÓ¡";
            icon = "ìå»êÑªÓ¡.dds";
            maxlevel = 20;
            maxlearn = 9;
            type = 2;
            eventflag = 0;
            spcost = 1;
            timetype = 0;
            targettype = 0;
            autoattack = 0;
            allowform = 2;
            credittype = 0;
            serialskill = 0;
            charge = 0;
            chargedist = 0;
            succeedskillcolor = 1;
            succeedskillcolornum = 1;
            comboskill = 0;
            cycle = 0;
            cyclegfx = "";
            cyclemode = 0;
            skill_class = 4;
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
            talent[0] = 1168;
            talent[1] = 1169;
            talent[2] = 1170;
            talent[3] = 1570;
            talent[4] = 1111;
            talent[5] = 1117;
            talent[6] = 1134;
            talent_size = 7;
            action[0] = "ìå»êÑªÓ¡";
            action[1] = "ìå»êÑªÓ¡";
            action[2] = "ìå»êÑªÓ¡";
            action[3] = "ìå»êÑªÓ¡";
            action[4] = "ìå»êÑªÓ¡";
            action[5] = "ìå»êÑªÓ¡";
            action[6] = "0";
            action[7] = "0";
            action[8] = "0";
            action[9] = "0";
            action[10] = "0";
            action[11] = "0";
            action[12] = "0";
            action[13] = "";
            action[14] = "";
            action[15] = "ìå»êÑªÓ¡";
            action[16] = "ìå»êÑªÓ¡";
            action[17] = "ìå»êÑªÓ¡";
            action[18] = "ìå»êÑªÓ¡";
            action[19] = "ìå»êÑªÓ¡";
            action[20] = "ìå»êÑªÓ¡";
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
            dobless = false;
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
        virtual ~ Skill1124Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        int GetPreSkillID (int index) const
        {
            static int array[1] = { 1122 };
            return array[index];
        }
        int GetPreSkillSP (int index) const
        {
            static int array[1] = { 1 };
            return array[index];
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (0);
        }
        int GetExecutetime (Skill * skill) const
        {
            return 1000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 180000 - 18000 * skill->GetT0 ();
        }
        int GetRequiredLevel (int level) const
        {
            static int array[9] = { 106, 109, 112, 115, 118, 121, 124, 125, 125 };
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
            return _snwprintf (buffer, length, format, 0, 27 + 3 * skill->GetLevel (), 33, skill->GetLevel ());

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
            skill->GetVictim ()->SetProbability (1.0 * (33 + 5 * skill->GetT1 ()));
            skill->GetVictim ()->SetTime ((27000 + 100 + 3000 * skill->GetLevel () + 2000 * skill->GetT2 ()) *
                                          (1 + norm (zrand (100) / (100 - 10 * skill->GetT2 ()))));
            skill->GetVictim ()->SetRatio (0.01 * skill->GetLevel () + 0.01 * skill->GetT1 ());
            skill->GetVictim ()->SetAttack2hp (1);
            skill->GetVictim ()->SetTime (5050 * (1 + norm (zrand (100) / (100 - 10 * skill->GetT2 ()))));
            skill->GetVictim ()->SetRatio (0.07);
            skill->GetVictim ()->SetInccritrate (1);
            skill->GetVictim ()->SetProbability (1.0 * (120 * norm (skill->GetT1 ())));
            skill->GetVictim ()->SetTime (30000);
            skill->GetVictim ()->SetRatio (1570);
            skill->GetVictim ()->SetBuffid (13);
            skill->GetVictim ()->SetAmount (1111);
            skill->GetVictim ()->SetValue (1117);
            skill->GetVictim ()->SetIncskilllevel (1);
            return true;
        }
#endif
    };
}
#endif
