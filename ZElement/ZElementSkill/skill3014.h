#ifndef __CPPGEN_GNET_SKILL3014
#define __CPPGEN_GNET_SKILL3014
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill3014:public Skill
    {
      public:
        enum
        { SKILL_ID = 3014 };
          Skill3014 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill3014Stub:public SkillStub
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
            bool Cancel (Skill * skill) const
            {
                return 1;
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
      Skill3014Stub ():SkillStub (3014)
        {
            occupation = 167;
            name = L"ÓñÕæ¾÷<Ðþ>";
            nativename = "ÓñÕæ¾÷<Ðþ>";
            icon = "ÏÉ¡¤ÓñÕæ¾÷.dds";
            maxlevel = 20;
            maxlearn = 20;
            type = 3;
            eventflag = 0;
            spcost = 0;
            timetype = 0;
            targettype = 0;
            autoattack = 0;
            allowform = 0;
            credittype = 0;
            clearmask = 1;
            serialskill = 0;
            charge = 0;
            chargedist = 0;
            succeedskillcolor = 1;
            succeedskillcolornum = 1;
            comboskill = 0;
            cycle = 0;
            cyclegfx = "";
            cyclemode = 0;
            skill_class = -2;
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
            talent[0] = 1545;
            talent_size = 1;
            action[0] = "ÇàÔÆÃÅ_ÓñÕæ¾÷";
            action[1] = "ÇàÔÆÃÅ_ÓñÕæ¾÷";
            action[2] = "¹íÍõ×Ú_ÓñÕæ¾÷";
            action[3] = "ºÏ»¶ÅÉ_ÓñÕæ¾÷";
            action[4] = "ÌìÒôËÂ_ÓñÕæ¾÷";
            action[5] = "¹íµÀ_ÓñÕæ¾÷";
            action[6] = "·ÙÏã¹È_ÓñÕæ¾÷";
            action[7] = "ÊÞÉñ_ÓñÕæ¾÷";
            action[8] = "ºüÑý_ÓñÕæ¾÷";
            action[9] = "ÐùÔ¯_ÓñÕæ¾÷";
            action[10] = "»³¹â_ÓñÕæ¾÷";
            action[11] = "Ì«ê»_ÓñÕæ¾÷";
            action[12] = "Ìì»ª_ÓñÕæ¾÷";
            action[13] = "ÁéÙí_ÓñÕæ¾÷";
            action[14] = "Ó¢ÕÐ_ÓñÕæ¾÷";
            action[15] = "ÇàÔÆÃÅ_Æï³Ë_ÓñÕæ¾÷";
            action[16] = "ÇàÔÆÃÅ_Æï³Ë_ÓñÕæ¾÷";
            action[17] = "¹íÍõ×Ú_Æï³Ë_ÓñÕæ¾÷";
            action[18] = "ºÏ»¶ÅÉ_Æï³Ë_ÓñÕæ¾÷";
            action[19] = "ÌìÒôËÂ_Æï³Ë_ÓñÕæ¾÷";
            action[20] = "¹íµÀ_Æï³Ë_ÓñÕæ¾÷";
            action[21] = "·ÙÏã¹È_Æï³Ë_ÓñÕæ¾÷";
            action[22] = "ÊÞÉñ_Æï³Ë_ÓñÕæ¾÷";
            action[23] = "ºüÑý_Æï³Ë_ÓñÕæ¾÷";
            action[24] = "ÐùÔ¯_Æï³Ë_ÓñÕæ¾÷";
            action[25] = "»³¹â_Æï³Ë_ÓñÕæ¾÷";
            action[26] = "Ì«ê»_Æï³Ë_ÓñÕæ¾÷";
            action[27] = "Ìì»ª_Æï³Ë_ÓñÕæ¾÷";
            action[28] = "ÁéÙí_Æï³Ë_ÓñÕæ¾÷";
            action[29] = "Ó¢ÕÐ_Æï³Ë_ÓñÕæ¾÷";
            action[30] = "0";
            rangetype = 0;
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
        virtual ~ Skill3014Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (180 + 30 * skill->GetLevel () + 3 * skill->GetLevel () * skill->GetLevel ());
        }
        int GetPreSkillID (int index) const
        {
            static int array[1] = { 1525 };
            return array[index];
        }
        int GetPreSkillSP (int index) const
        {
            static int array[1] = { 10 };
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
        float GetEffectdistance (Skill * skill) const
        {
            return (float) (18);
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (1250 + 50 * skill->GetLevel ());
        }
        int GetExecutetime (Skill * skill) const
        {
            return 1000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 900000 - 27000 * skill->GetT0 () - 20000 * skill->GetLevel ();
        }
        int GetRequiredLevel (int level) const
        {
            static int array[20] = { 120, 122, 124, 126, 128, 130, 131, 132, 133, 134, 135, 136, 137, 138, 139, 141, 143, 145, 147, 149 };
            return array[level - 1];
        }
        float GetPraydistance (Skill * skill) const
        {
            return (float) (15);
        }
        float GetCastdistance (Skill * skill) const
        {
            return (float) (12);
        }
#ifdef _SKILL_CLIENT
        int GetDescription (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format,
                               1250 + 50 * skill->GetLevel (),
                               180 + 30 * skill->GetLevel () + 3 * skill->GetLevel () * skill->GetLevel (),
                               900 - 20 * skill->GetLevel (), 12, 4 + skill->GetLevel (), 2 * skill->GetLevel (), skill->GetLevel ());

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
            skill->GetVictim ()->SetTime (100);
            skill->GetVictim ()->SetBuffid (1);
            skill->GetVictim ()->SetAmount (skill->GetPlayer ()->
                                            GetIsfrozen ()? (skill->GetPlayer ()->GetAtkrate () + skill->GetPlayer ()->GetDodge ()) * (4 +
                                                                                                                                       skill->
                                                                                                                                       GetLevel ()) +
                                            norm (skill->GetLevel () / 19.9) * 0.3 * skill->GetPlayer ()->GetMaxmp () : (1 +
                                                                                                                         norm (zrand
                                                                                                                               (skill->GetLevel () +
                                                                                                                                100) / 100)) *
                                            ((skill->GetPlayer ()->GetAtkrate () + skill->GetPlayer ()->GetDodge ()) * (4 + skill->GetLevel ()) +
                                             norm (skill->GetLevel () / 19.9) * 0.3 * skill->GetPlayer ()->GetMaxmp ()));
            skill->GetVictim ()->SetValue (0);
            skill->GetVictim ()->SetHpleak2 (1);
            return true;
        }
        bool BlessMe (Skill * skill) const
        {
            skill->GetVictim ()->SetRatio (1520 + 8 * norm (zrand (100) / (100 - 2 * skill->GetLevel ())));
            skill->GetVictim ()->SetAmount (1520 + 9 * norm (zrand (100) / (100 - 2 * skill->GetLevel ())));
            skill->GetVictim ()->SetValue (1520 + 10 * norm (zrand (100) / (100 - 2 * skill->GetLevel ())));
            skill->GetVictim ()->SetClearcooldown (1);
            skill->GetVictim ()->SetProbability (1.0 * (120));
            skill->GetVictim ()->SetTime (900000 - 20000 * skill->GetLevel ());
            skill->GetVictim ()->SetRatio (1525);
            skill->GetVictim ()->SetAmount (1525);
            skill->GetVictim ()->SetValue (1525);
            skill->GetVictim ()->SetSetcooldown (1);
            return true;
        }
#endif
    };
}
#endif
